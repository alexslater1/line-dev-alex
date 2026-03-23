from line_solver import *


def json_join_deadline():
    """Closed 1-class fork-join with quorum join and deadline."""
    model = Network('JoinDeadline')

    delay = Delay(model, 'Delay')
    fork = Fork(model, 'Fork')
    queue1 = Queue(model, 'Queue1', SchedStrategy.FCFS)
    queue2 = Queue(model, 'Queue2', SchedStrategy.FCFS)
    queue3 = Queue(model, 'Queue3', SchedStrategy.FCFS)
    join = Join(model, 'Join', fork)

    class1 = ClosedClass(model, 'Class1', 5, delay, deadline=5.0)

    delay.setService(class1, Exp(1))
    queue1.setService(class1, Exp(3))
    queue2.setService(class1, Exp(4))
    queue3.setService(class1, Exp(2))

    join.obj.setStrategy(class1.obj, JoinStrategy.Quorum.value)
    join.obj.setRequired(class1.obj, 2)

    P = model.initRoutingMatrix()
    P.set(class1, class1, delay, fork, 1.0)
    P.set(class1, class1, fork, queue1, 1.0)
    P.set(class1, class1, fork, queue2, 1.0)
    P.set(class1, class1, fork, queue3, 1.0)
    P.set(class1, class1, queue1, join, 1.0)
    P.set(class1, class1, queue2, join, 1.0)
    P.set(class1, class1, queue3, join, 1.0)
    P.set(class1, class1, join, delay, 1.0)
    model.link(P)
    return model
