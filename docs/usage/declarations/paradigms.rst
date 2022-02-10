Paradigms declaration
===========================================

A `paradigm declaration` is a special case of a category declaration, so its accepts language string attributes.

To declare a paradigm in IEML, you use the :code:`@paranode` declaration. ::

    @word "wa.".
    @word "e." .
    @word "a." .

    @node
        en: root
        (
            0 #"wa."
        ).

    @node 
        en: varation1
        (
            0 #"e."
        ).

    @node
        en: variation2
        ( 
            0 #"a."
        ).

    @paranode 
        en: paradigm
        1d: /#/1
        (
            0 #root,
            1 #{varation1; variation2}
        ).

A `paradigm declaration` must declare a paradigm, meaning that the phrase declared cannot be singular, 
it has to have at least one substitution group with distincts values.

Invariant definition
----------------------------------------

A `paradigm declaration` must have an invariant declared as a :code:`node`. 
The invariant of a paradigm is the phrase composed by all the roles of the phrase where there is no substitution group.
For the previous `paradigm`, the invariant must be declared like so ::

    @node 
        en: paradigm invariant
        (
            0 #root
        ).

Dimension attribute
-----------------------------------------

The dimension attributes :code:`1d`, :code:`2d` and :code:`3d` allows the user to defines a table structure alongside the paradigm. 
If the dimension attribute is present, the paradigm can be displayed on either a `1D`, `2D` or `3D` grid. 
Its singular sequences get implicit coordinates in this grid. 

In the previous `paradigm`, the :code:`1d: /#/1` attribute indicates that the dimension of variation in role :code:`1` of the singular sequence of the paradigm correspond to the rows of a table. 
It accepts a :code:`;` separated list of :code:`path` literals as value.


