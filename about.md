# PythonThingIDK

- Lets you make small mods using python.


```python
def MenuLayer_init(this):
    if not this.init(): # calling original, `this` is the current instance of MenuLayer, well its the hook of menulayer this wouldve crashed if `init` wasnt virtual
        return False
    
    node1 = CCNode.create()
    node1.setID("hello-node")
    node2 = CCNode.create()
    node2.setID("hello-two")
    node1.addChild(node2)
    this.addChild(node1)

    return True
```