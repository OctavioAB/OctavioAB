from __future__ import annotations # pretty sure this is no longer needed starting 3.14

class LinkedList:
    ''' The same as we have already used in the notes for this chapter '''
    class ListNode:
        def __init__(self, data):
            self.data = data
            self.next: ListNode = None
        def __str__(self):
            return f"ListNode data: {self.data}"
    
    def __init__(self):
        self.head: ListNode = None
        self.tail: ListNode = None

    def append(self, data) -> None:
        ''' Add a node at the "end" of the list'''
        new_node = self.ListNode(data)
        if not self.head:
            self.head = new_node
            self.tail = new_node
            return
        current = self.head
        while current.next:
            current = current.next
        current.next = new_node
        self.tail = new_node
        
    def __contains__(self, data) -> bool:
        '''Return true if there is a ListNode with data.
            Returns: 
            bool: True if data is in this list, False otherwise'''
        if self.head == None:
            return False
        current: ListNode = self.head
        while current and current.data is not data:
            current = current.next
        if current and current.data == data:
            return True
        return False
    
    def __str__(self) -> str:
        '''Prints list in order starting from head element'''
        if self.head == None:
            return f"LinkedList: []"
        to_return = f"LinkedList: ["
        current: ListNode = self.head
        while current:
            to_return = to_return + f" {current.data}"
            current = current.next
            if current:
                to_return = to_return + ", "
        return to_return + " ]"
    