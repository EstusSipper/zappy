from enum import Enum

class Ressouces(Enum):
    FOOD = 1
    LINEMATE = 2
    DERAUMERE = 3
    SIBUR = 4
    MENDIANE = 5
    PHIRAS = 6
    THYSTAME = 7

class Inventory:
    def __init__(self):
        self.food = 10
        self.linemate = 0
        self.deraumere = 0
        self.sibur = 0
        self.mendiane = 0
        self.phiras = 0
        self.thystame = 0
    def add_ressource(self, ressource : Ressouces, amount : int):
        if ressource == Ressouces.FOOD:
            self.food += amount
        elif ressource == Ressouces.LINEMATE:
            self.linemate += amount
        elif ressource == Ressouces.DERAUMERE:
            self.deraumere += amount
        elif ressource == Ressouces.SIBUR:
            self.sibur += amount
        elif ressource == Ressouces.MENDIANE:
            self.mendiane += amount
        elif ressource == Ressouces.PHIRAS:
            self.phiras += amount
        elif ressource == Ressouces.THYSTAME:
            self.thystame += amount
        else:
            raise Exception("Invalid ressource")
    def remove_ressource(self, ressource : Ressouces, amount : int):
        if ressource == Ressouces.FOOD:
            if self.food < amount:
                raise Exception("Not enough food")
            self.food -= amount
        elif ressource == Ressouces.LINEMATE:
            if self.linemate < amount:
                raise Exception("Not enough linemate")
            self.linemate -= amount
        elif ressource == Ressouces.DERAUMERE:
            if self.deraumere < amount:
                raise Exception("Not enough deraumere")
            self.deraumere -= amount
        elif ressource == Ressouces.SIBUR:
            if self.sibur < amount:
                raise Exception("Not enough sibur")
            self.sibur -= amount
        elif ressource == Ressouces.MENDIANE:
            if self.mendiane < amount:
                raise Exception("Not enough mendiane")
            self.mendiane -= amount
        elif ressource == Ressouces.PHIRAS:
            if self.phiras < amount:
                raise Exception("Not enough phiras")
            self.phiras -= amount
        elif ressource == Ressouces.THYSTAME:
            if self.thystame < amount:
                raise Exception("Not enough thystame")
            self.thystame -= amount
        else:
            raise Exception("Invalid ressource")
    def get_ressource(self, ressource : Ressouces):
        if ressource == Ressouces.FOOD:
            return self.food
        elif ressource == Ressouces.LINEMATE:
            return self.linemate
        elif ressource == Ressouces.DERAUMERE:
            return self.deraumere
        elif ressource == Ressouces.SIBUR:
            return self.sibur
        elif ressource == Ressouces.MENDIANE:
            return self.mendiane
        elif ressource == Ressouces.PHIRAS:
            return self.phiras
        elif ressource == Ressouces.THYSTAME:
            return self.thystame
        else:
            raise Exception("Invalid ressource")

    def print_inventory(self):
        print("food: " + str(self.food))
        print("linemate: " + str(self.linemate))
        print("deraumere: " + str(self.deraumere))
        print("sibur: " + str(self.sibur))
        print("mendiane: " + str(self.mendiane))
        print("phiras: " + str(self.phiras))
        print("thystame: " + str(self.thystame))
##class player:

class Player:
    def __init__(self):
        self.level = 1
        self.inventory = Inventory()

p = Player()
# p.inventory.print_inventory()
p.inventory.add_ressource(Ressouces.FOOD, 10)
# p.inventory.print_inventory()
p.inventory.add_ressource(Ressouces.DERAUMERE, 5)
p.inventory.print_inventory()