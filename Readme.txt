
Input/output del software
------------------------------
File dot in input, file dot in output



Semplificazioni rispetto alla grammatica in https://graphviz.org/doc/info/lang.html
------------------------------------------------------------------------------------------------

Non si utilizzano ne' GRAPH ne' SUBGRAPH, ammesso solo DIGRAPH

Per i nodi sono ammessi solo i seguenti attributi:
fontname
shape
colorscheme
label
fillcolor
style


Per gli archi sono ammessi solo i seguenti attributi:
fontname
label
style


Convenzioni per stati iniziali/finali degli automi
------------------------------------------------------

Lo stato iniziale e' il nodo 0.
Gli stati finali sono quelli con shape=doublecircle.
eps sta per la parola vuota epsilon.


Esempio
----------
Trovate un automa di esempio in esempio.dot



Documento accompagnatorio
----------------------------------
Deve contenere istruzioni per la compilazione e qualunque altra info ritenuta
interessante.
