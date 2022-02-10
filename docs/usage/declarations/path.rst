Path expression
==========================

A path expression represents a position in a given phrase.
A path is composed of :code:`path nodes` separated by :code:`/` slashs.


Path Nodes
----------------------------------

There is a type of path node for all the possible syntactic position in a phrase

.. table:: Path nodes specification

  ==================  ==================================================   ==================================================================================================================================================================================== ============================================================================================================================================================
    Path Node Type    Form                                                  Meaning                                                                                                                                                                               Accept next                
  ==================  ==================================================   ==================================================================================================================================================================================== ============================================================================================================================================================
  **Root**               :code:`#`                                            Represents a phrase.                                                                                                                                                               **RoleNumber**, **Junction**
  **RoleNumber**         :code:`%d`                                           Represents the phrase line with role :code:`%d` in a phrase.                                                                                                                       **Junction**, **Auxiliary**, **InflectionList**, **Word**, **Root**
  **Paradigm**           :code:`{%d}`                                         Represents the :code:`%d` singular sequence of a phrase.                                                                                                                           **Root**
  **Auxiliary**          :code:`*%word` :code:`*%identifier`                  Represents the flexed subphrase line with auxiliary identified by :code:`*%word` or :code:`*%identifier` in a phrase line.                                                         **Junction**, **InflectionList**, **Word**, **Root**
  **InflectionList**     :code:`~%word~%word...` :code:`~%identifier...`      Represents the category with flexing list :code:`~%word~%word...` or :code:`~%identifier...` in a flexed subphrase line.                                                           **Junction**, **Word**, **Root**
  **Word**               :code:`%word`                                        Represents the category word `%word`.                                                                                                                                              nothing
  **Junction**           :code:`%word[%d]` :code:`%identifier[%d]`            Represents the phrase or sub phrase line at position :code:`%d` in the :code:`%word[%d]` or :code:`%identifier[%d]` junction.                                                      Same as the element preceding the junction path node, except another **Junction** node.
  ==================  ==================================================   ==================================================================================================================================================================================== ============================================================================================================================================================


.. topic:: Legend  

  :code:`%d` represents a digit.
  :code:`%word` represents a script word.
  :code:`%identifier` represents an identifier.


Examples
--------------------------------------


Case 1 : Phrase with two 6 roles
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


Case 2 : Paradigm
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
::

    (
        0 #root,
        1 {#first variation; #second variation},
        2 {~first inflection; ~second inflection} #object
    )

======================================= ===========================================================================
Path                                     Pointed subphrase
======================================= ===========================================================================
:code:`/#/0`                             *invalid path* : must specify a singular sequence coordinate.
:code:`/#/{0}/0`                         :code:`#root`
:code:`/#/{1}/0`                         :code:`#root`
:code:`/#/{5}/0`                         *invalid path* : only four singular sequences.
:code:`/#/{0}/1`                         :code:`#first variation`
:code:`/#/{0}/2`                         :code:`~first inflection #object`
:code:`/#/{1}/2`                         :code:`~second inflection #object`
======================================= ===========================================================================


