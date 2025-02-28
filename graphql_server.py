import strawberry
from fastapi import FastAPI
from strawberry.fastapi import GraphQLRouter
from typing import List
import os


#simple method to strip register name, and values from output txt file
def read_registers():
    registers = {}
    try:
        with open("cpu_registers_output.txt", 'r') as f:
            lines = f.readlines()
            for line in lines:
                if line.startswith("Register"):
                    reg_name, values = line.split(":")
                    reg_name = reg_name.strip()
                    values = values.strip().split(" ")[1:]
                    registers[reg_name] = list(map(int, values))
                    print(reg_name, values)
                    
    except Exception as e:
        print(f"Error reading file: {e}")

    return registers

#GraphQL time yay
@strawberry.type
class Query:
    @strawberry.field
    def get_register(self, register:str) -> List[int]:
        registers = read_registers()
        return registers.get(register, [])

#defining schema using strawberry and Query we just wrote
schema = strawberry.Schema(query=Query)

app = FastAPI()
graphql_app = GraphQLRouter(schema)
app.include_router(graphql_app, prefix = "/graphql")
