class Branches:
    def __init__(self):
        self.dict = {}

    def set(self, key, location):
        self.dict[key] = location
    
    def get(self, key):
        return self.dict[key]