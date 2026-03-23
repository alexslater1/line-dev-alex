from line_solver import *


def json_hetero_servers():
    """Closed 2-class network with heterogeneous servers."""
    model = Network('HeteroServers')

    delay = Delay(model, 'Delay')
    queue = Queue(model, 'Queue', SchedStrategy.FCFS)
    queue.setNumberOfServers(3)

    class1 = ClosedClass(model, 'Class1', 3, delay)
    class2 = ClosedClass(model, 'Class2', 2, delay)

    delay.setService(class1, Exp(1))
    delay.setService(class2, Exp(2))

    # Default service (required before hetero setup)
    queue.setService(class1, Exp(3))
    queue.setService(class2, Exp(2))

    # Heterogeneous server types
    fast = ServerType('Fast', 2, [class1, class2])
    slow = ServerType('Slow', 1, [class1])

    queue.addServerType(fast)
    queue.addServerType(slow)

    queue.setHeteroSchedPolicy(HeteroSchedPolicy.ORDER)

    queue.setHeteroService(class1, fast, Exp(5))
    queue.setHeteroService(class2, fast, Exp(3))
    queue.setHeteroService(class1, slow, Exp(1))

    P = model.initRoutingMatrix()
    P.set(class1, class1, delay, queue, 1.0)
    P.set(class1, class1, queue, delay, 1.0)
    P.set(class2, class2, delay, queue, 1.0)
    P.set(class2, class2, queue, delay, 1.0)
    model.link(P)
    return model
