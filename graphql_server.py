import strawberry
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from strawberry.fastapi import GraphQLRouter
from typing import List, Dict, Optional
import asyncio
import json
import os
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

#global variable to store the latest register data
current_registers = {}

#file path, can be named anything
REGISTER_FILE = "cpu_registers_output.txt"

# File watcher class
class RegisterFileHandler(FileSystemEventHandler):
    def on_modified(self, event):
        if event.src_path.endswith(REGISTER_FILE):
            global current_registers
            current_registers = read_registers()

#function to read register data from file
def read_registers() -> Dict[str, List[int]]:
    registers = {}
    try:
        with open(REGISTER_FILE, 'r') as f:
            lines = f.readlines()
            for line in lines:
                #take out the registers, and the values for each
                if line.startswith("Register"):
                    parts = line.split(":")
                    if len(parts) >= 2:
                        reg_name = parts[0].replace("Register", "").strip()
                        values_str = parts[1].split("[")[1].split("]")[0].strip()
                        if values_str:
                            try:
                                values = [int(v) for v in values_str.split()]
                                registers[reg_name] = values
                            except ValueError:
                                registers[reg_name] = []
    except Exception as e:
        print(f"Error reading file: {e}")
    
    return registers

#set up the file watcher using os.path.dirname
def setup_file_watcher():
    event_handler = RegisterFileHandler()
    observer = Observer()
    observer.schedule(event_handler, path=os.path.dirname(os.path.abspath(REGISTER_FILE)) or '.', recursive=False)
    observer.start()
    return observer

#graphQL time yayyy
@strawberry.type
class Register:
    name: str
    values: List[int]

@strawberry.type
class Query:
    @strawberry.field
    def get_register(self, name: str) -> Optional[List[int]]:
        global current_registers
        return current_registers.get(name, [])
    
    @strawberry.field
    def get_all_registers(self) -> List[Register]:
        global current_registers
        return [Register(name=name, values=values) for name, values in current_registers.items()]

schema = strawberry.Schema(query=Query)

#fastAPI app for uvicorn server
app = FastAPI()
graphql_app = GraphQLRouter(schema)
app.include_router(graphql_app, prefix="/graphql")

#webSocket for real-time updates of the numbers
@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    try:
        while True:
            #send current register data every second using send_text
            await websocket.send_text(json.dumps(current_registers))
            #1 sec delay
            await asyncio.sleep(1)
    except WebSocketDisconnect:
        pass

#startup event to initialize the file watcher
@app.on_event("startup")
async def startup_event():
    global current_registers
    current_registers = read_registers()
    app.state.observer = setup_file_watcher()

#shutdown event to stop the file watcher
@app.on_event("shutdown")
async def shutdown_event():
    app.state.observer.stop()
    app.state.observer.join()
