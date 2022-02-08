Path expression
==========================

A path expression represents a position in a given phrase.
A path is composed of :code:`path nodes` separated by :code:`/` slashs.


Path Nodes
----------------------------------

There is a type of path node for all the possible syntactic position in a phrase


==================  ==================================================   ====================================================================================================================================================================================
  Path Node Type    Form                                                  Meaning
==================  ==================================================   ====================================================================================================================================================================================
**Root**               :code:`#`                                            Represents a phrase.
**RoleNumber**         :code:`%d`                                           Represents the phrase line with role :code:`%d` in a phrase.
**Paradigm**           :code:`{%d}`                                         Represents the :code:`%d` singular sequence of a phrase.
**Auxiliary**          :code:`*%word` :code:`*%identifier`                  Represents the flexed subphrase line with auxiliary identified by :code:`*%word` or :code:`*%identifier` in a phrase line.
**InflectionList**     :code:`~%word~%word...` :code:`~%identifier...`      Represents the category with flexing list :code:`~%word~%word...` or :code:`~%identifier...` in a flexed subphrase line.
**Word**               :code:`%word`                                        Represents the category word `%word`.
**Junction**           :code:`%word[%d]` :code:`%identifier[%d]`            Represents the phrase or sub phrase line at position :code:`%d` in the :code:`%word[%d]` or :code:`%identifier[%d]` junction.
==================  ==================================================   ====================================================================================================================================================================================

Examples
--------------------------------------


Case 1
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
::

    (
        0 #root,
        6 *above ~singular #item above
        6 *below #item below
    )

======================================= ===========================================================================
Path                                     Pointed subphrase
======================================= ===========================================================================
:code:`/#/6/*above/~singular`            :code:`#item above`
:code:`/#/6/*above`                      :code:`~singular #item above`
:code:`/#/6/*below`                      :code:`#item below`
:code:`/#/6`                             :code:`*above ~singular #item above` and :code:`*below #item below`
:code:`/#/1`                             nothing
:code:`/0`                               *invalid path*
======================================= ===========================================================================