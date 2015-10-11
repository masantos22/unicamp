import graphviz as gv
import functools
from os import listdir

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

path = '../Alocacao_Propaganda/backtracking/data/'
files = listdir(path)
print files

for i in range(0,len(files) - 1,2):
    nodes = []
    edges = []
    nodes_id = []
    fname = files[i].split('_')[0]

    with open(path + files[i+1]) as f:
        k = 0
        for line in f.readlines():
            k += 1
            print line[:-1]
            content = line[:-1].split(',')
            nodes.append((content[0],{'label':'Ordem: '+str(k)+'\nProximo: '+content[0].split('_')[0]+'\nPropagandas: '+content[0].split('_')[1]+'\nCusto: '+content[1]+'\nLucro: '+content[2]}))
            nodes_id.append(content[0])
            if k > 19:
               break

    with open(path + files[i]) as f:
        for line in f.readlines():
            content = line[:-1].split(',')
            if content[0] in nodes_id and content[1] in nodes_id:
                edges.append((content[0],content[1]))
    
    g6 = add_edges(
    add_nodes(digraph(),nodes),edges).render('../Alocacao_Propaganda/backtracking/test/'+fname)
