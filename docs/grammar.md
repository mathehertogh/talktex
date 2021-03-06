# TalkTex grammar reference

This document defines the complete TalkTex grammar. Read the
[user guide](guide.md) for an explanation.

## Keywords

Here is a complete list of TalkTex keywords.

- capital
- of
- from
- to
- function
- fraction
- over
- maps
- mapping
- open
- close
- parenthesis
- end
- ...

---
**NOTE**: The "..." keyword is not an actual *word*: it denotes a (not too
small) moment of silence during speech. TalkTex will replace each such silence
by the keyword "end". Read [this section](guide.md#Pauses-in-speech) of the user
guide for more info.

---

## Constructs

Below we define all constructs of TalkTex.
**NOTE**: The defined synonyms are not implemented yet

### DIGIT
| phrase | target | priority | synonyms |
|--------|--------|----------|----------|
| zero   | 0      | 10       |          |
| one    | 1      | 10       |          |
| two    | 2      | 10       |          |
| three  | 3      | 10       |          |
| four   | 4      | 10       |          |
| five   | 5      | 10       |          |
| six    | 6      | 10       |          |
| seven  | 7      | 10       |          |
| eight  | 8      | 10       |          |
| nine   | 9      | 10       |          |

### SMALLLETTER
| phrase    | target | priority | synonyms |
|-----------|--------|----------|----------|
| a         | a      | 12       |          |
| b         | b      | 12       |          |
| etc.      | etc.   | etc.     | etc.     |
| z         | z      | 12       |          |
| alpha     | \alpha | 12       |          |
| beta      | \beta  | 12       |          |
| etc.      | etc.   | etc.     | etc.     |
| omega     | \omega | 12       |          |

### LETTER
| phrase                 | target                 | priority | synonyms |
|------------------------|------------------------|----------|----------|
| capital SMALLLETTER[L] | \MakeUppercase{L}      | 11       |          |
| SMALLLETTER[L]         | L                      | 10       |          |

### TYPESETTING
| phrase       | target    | priority | synonyms |
|--------------|-----------|----------|----------|
| bold         | \mathbb   | 10       |          |
| calligraphic | \mathcal  | 10       |          |
| fraktur      | \mathfrak | 10       |          |

### ACCENT
| phrase | target | priority | synonyms |
|--------|--------|----------|----------|
| tilde  | \tilde | 10       |          |
| hat    | \hat   | 10       |          |
| bar    | \bar   | 10       |          |

### VARIABLE
| phrase                     | target | priority | synonyms |
|----------------------------|--------|----------|----------|
| LETTER[L]                  | L      | 9        |          |
| TYPESETTING[T] VARIABLE[V] | T{ V } | 8        |          |
| VARIABLE[V] ACCENT[A]      | A{ V } | 7        |          |

### SYMBOL
| phrase      | target    | priority | synonyms |
|-------------|-----------|----------|----------|
| empty set   | \emptyset | 10       |          |
| infinity    | \infty    | 10       |          |
| DIGIT[D]    | D         | 9        |          |
| VARIABLE[V] | V         | 6        |          |

### UNOP
| phrase      | target  | priority | synonyms  |
|-------------|---------|----------|-----------|
| square root | \sqrt   | 10       |           |
| sin         | \sin    | 10       | sine      |
| cos         | \cos    | 10       | cosine    |
| tang        | \tan    | 10       | tangent   |
| exp         | \exp    | 10       | exponent  |
| log         | \log    | 10       | logarithm |
| negate      | \neg    | 10       | negated   |
| for all     | \forall | 10       |           |
| exists      | \exists | 10       |           |

### BINOP
| phrase        | target          | priority | synonyms                   |
|---------------|-----------------|----------|----------------------------|
| plus          | +               | 10       |                            |
| minus         | -               | 10       |                            |
| times         | \cdot           | 10       |                            |
| power         | ^               | 10       |                            |
| divided by    | /               | 10       | pipe                       |
| divides       | \mid            | 10       |                            |
| equals        | =               | 10       | equal                      |
| isomorphic    | \cong           | 10       |                            |
| less          | <               | 10       | smaller                    |
| greater       | >               | 10       | bigger, larger             |
| less equal    | \leq            | 11       | smaller equal              |
| greater equal | \geq            | 11       | bigger equal, larger equal |
| and           | \wedge          | 10       |                            |
| or            | \vee            | 10       |                            |
| implies       | \implies        | 10       |                            |
| equivalent    | \Leftrightarrow | 10       |                            |
| union         | \cup            | 10       |                            |
| intersection  | \cap            | 10       | intersected                |
| set minus     | \setminus       | 10       | set difference             |
| subset        | \subset         | 10       | included in                |
| in            | \in             | 10       | element of                 |
| not BINOP[O]  | \not O          | 9        |                            |

### RANGEOP
| phrase   | target | priority | synonyms |
|----------|--------|----------|----------|
| sum      | \sum   | 10       |          |
| product  | \prod  | 10       |          |
| integral | \int   | 10       |          |

### RANGE
| phrase                  			| target    | priority | synonyms |
|-----------------------------------|-----------|----------|----------|
| from OPENEXPR[A] to ANYEXPR[B] 	| \_{A}^{B} | 5        |          |

### FRAC
| phrase                        | target      | priority | synonyms |
|-------------------------------|-------------|----------|----------|
| fraction EXPR[A] over EXPR[B] | \frac{A}{B} | 5        |          |

### OPENFUNC
| phrase                                           | target     | priority   | synonyms |
|--------------------------------------------------|------------|------------|----------|
| function VARIABLE[F] from SYMBOL[A] to SYMBOL[B] | F: A \to B | 5          |          |

### MAPSTO
| phrase                  		 | target      | priority | synonyms                   |
|--------------------------------|-------------|----------|----------------------------|
| maps OPENEXPR[X] to ANYEXPR[Y] | X \mapsto Y | 5        | mapping OPENEXPR[X] to ANYEXPR[Y] |

### FUNC
| phrase                | target | priority | synonyms |
|-----------------------|--------|----------|----------|
| OPENFUNC[F] MAPSTO[M] | F, M   | 4        |          |
| OPENFUNC[F]           | F      | 3        |          |

### EXPR
| phrase                                     		| target            | priority | synonyms |
|---------------------------------------------------|-------------------|----------|----------|
| SYMBOL[S]                                  		| S                 | 2        |          |
| FRAC[F]                                    		| F                 | 2        |          |
| FUNC[F]                                    		| F                 | 2        |          |
| UNOP[O] of OPENEXPR[A]                         	| O\left( A \right) | 2        |          |
| UNOP[O] SIMPLEEXPR[A]                            	| O A               | 2        |          |
| OPENEXPR[A] BINOP[O] ANYEXPR[B]              		| {A}O{B}           | 2        |          |
| RANGEOP[O] RANGE[R] ANYEXPR[A]                	| OR(A)             | 2        |          |
| open parenthesis OPENEXPR[A] close parenthesis	| \left( A \right)  | 2        |          |

### CLOSEDEXPR
| phrase          	| target | priority | synonyms |
|-------------------|--------|----------|----------|
| EXPR[A] end		| A      | 1        |          |

### OPENEXPR
| phrase          	| target | priority | synonyms |
|-------------------|--------|----------|----------|
| EXPR[A] 			| A      | 0        |          |

### ANYEXPR
| phrase          	| target | priority | synonyms |
|-------------------|--------|----------|----------|
| CLOSEDEXPR[A] 	| A      | 1        |          |
| OPENEXPR[A]     	| A      | 0        |          |
