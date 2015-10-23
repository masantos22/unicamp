import graphviz as gv
import functools
graph = functools.partial(gv.Graph, format='png')
digraph = functools.partial(gv.Digraph, format='png')


def add_nodes(graph, nodes):
    for n in nodes:
        if isinstance(n, tuple):
            graph.node(n[0], **n[1])
        else:
            graph.node(n)
    return graph

def add_edges(graph, edges):
    for e in edges:
        if isinstance(e[0], tuple):
            graph.edge(*e[0], **e[1])
        else:
            graph.edge(*e)
    return graph

'''
add_edges(
    add_nodes(digraph(), ['A', 'B', 'C']),
    [('A', 'B'), ('A', 'C'), ('B', 'C')]
).render('tree')
'''

g6 = add_edges(
    add_nodes(digraph(), [
        ('A', {'label': 'Custo: 20\nLucro: 40'}),
        ('B', {'label': 'Node B'}),
        'C'
    ]),
    [
        ('A', 'B'),
        ('A', 'C'),
        ('B', 'C')
    ]
).render('tree')