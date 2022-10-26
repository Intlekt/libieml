Format de la sortie JSON

```typescript
type Range = {
  file_id: string
  line_start: number
  line_end: number
  char_start: number
  char_end: number
}

type PositionedMessage = {
  range: Range
  message: string
}

// Tables
type TableBase = {
	// unique and stable id for this table
	id: string
	
	type: "CELL" | "TABLEND" | "TABLESET"
	
	// the type of the element id of the title, headers and cells attributes values (if present)
	element_type: "CATEGORY" | "SCRIPT"

	// element id of the title header of this table
	title: string
}

// Cell are table for singular element
// there are needed because a tableset can have a sum with a singular script 
// as an operand
// title is the element id of the content of the cell
type Cell = TableBase & {
	type: "CELL"
}

type TableNd = TableBase & {
	type: "TABLEND"

	// the number of dimension
	n_dim: 1 | 2 | 3
	
	// shape of this table
	//       len(shape) == n_dim
	shape: number[]
	
	// object id of the elements in row, column and tab header
	//       len(headers) == n_dim
	//       i < n_dim => len(hearders[i]) == shape[i]
	//
	// NOTE VIEW: only display the headers when n_dim == 2 or 3
	//       n_dim == 1 => headers[0] == cells
	headers: string[][]

	// cells
	//     i < n_dim => len(cells[a][b]..i times..[c]) == shape[i]
	//
	//      n_dim == 1 => i < shape[0], len(cells[i]) == 1 and len(cells[i][0]) == 1 and 
	//                                  cells[i][0][0] == headers[0][i] == title
  //      n_dim == 2 => i < shape[0], j < shape[1], len(cells[i][j]) == 1
	cells: string[][][]
}

type TableSet = TableBase & {
	type: "TABLESET"

	// list of table id referencing Cell or TableNd tables
	children: string[]
}

type Table = Cell | TableNd | TableSet

type ElementBase = {
  id: string
  range: Range
  translations: {
    FR: string[]
    EN: string[]
  },
  type: "WORD" | "CATEGORY" | "SCRIPT"
}

type Category = ElementBase & {
  category_type: "NODE" | "COMPONENT" | "PARADIGM" | "LINK"
  references: string[]
  back_references: string[]

	// number of singular sequence, if == 1 the category is singular, otherwise
	// it is a paradigm
	multiplicity: number

	// l'invariant d'une categorie est la category en commun a toutes les sequences
	// singulieres.
	singular_sequences: string[]
	
	// for paradigm, correspond to the node without variation
	// for node, corresponds to it self
	invariant: string

	// empty for a paradigm
	// contains all the ids of the paradigms where self is an invariant
	paradigms: string[]

	// table id of this category
	// NOTE: tables[table].element_type == "CATEGORY"
	//       multiplicity == 1 => table == null
  //       multiplicity != 1 => tables[table].type == "TABLEND"
	table: string | null


	// if not a LINK, the instances array will be empty
	// array of instance id (see parser output)
	instances: string[]

	// the word id that this category uses
  wordId: string	
}

type Word = ElementBase & {
  word_type: "CATEGORY" | "INFLECTION" | "JUNCTION" | "AUXILIARY"

	// the script id of this word 
	declaration: string
	
	// the category ids of the phrase that only use that word as category (and no
	// other category words). That is usefull to retrieve the translation of
	// category word for displaying
	phrase_words: string[]
}

type Script = ElementBase & {
	// string representation of the script
	str: string
	
	// the absolute position index in all the Script ordered
	index: number
	
	// depth of the multiplication tree
	layer: number

	// number of singular sequence, if == 1 the script is singular, otherwise
	// it is a paradigm
	multiplicity: number

	// list of script id that are the singular sequence of this script
	// if this script is singular, equals to [this.id] 
	singular_sequences: string[]

	script_type: "PRIMITIVE" | "NULLSCRIPT" | "ADDITION" | "MULTIPLICATION"
	
	// the word id of the word definition for this script if present.
	definition: string | null

	// the type of word that can be defined with this script
	declaration_type: "CATEGORY" | "INFLECTION" | "AUXILIARY" | "JUNCTION"
	
	// table id of this script
	// NOTE: tables[table].element_type == "SCRIPT"
	//       multiplicity == 1 => tables[table].type == "CELL"
	table: string
}

type Element = Word | Category | Script

type Instance = {
	id: string
	
	// category id
	link: string

	translations: {
    FR: string[]
    EN: string[]
  }
	
	// the word contained in this instance
	linkedWordIds: {[id: string]: string}
}

// a TableDefinition represents a predefined set of Paradigm to display in a
// recursive table layout.
type CategoryHierarchyDefinition = {
	// the id of the paradigm element that is the root (table initial layout) of
	// the table 
	root: string
	
	// range for this category_hierarchies definition
	range: Range

	// the invariant_mapping associate cells to paradigms
	// the id strings are category_id to singular sequence and the values are
  // category id to paradigms
	invariant_mapping: {[id: string]: string} 
}

type ParserOutput = {
  errors: PositionedMessage[]
  warnings: PositionedMessage[]
  language: "FR" | "EN"
  elements: { [id: string]: Element }
	tables: {[id: string]: Table}

	instances: {[id: string]: Instance}
	
	category_hierarchies: CategoryHierarchyDefinition[]
}
```
