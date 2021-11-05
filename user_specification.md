# User specification

This document explains what a user of TalkTex should say to obtain certain
Latex output.


## Examples

| Speech | Latex |
| ----------- | ----------- |
| a plus b | a + b |
| for all alpha bigger than one we have zero is smaller than alpha | \forall \alpha > 1 : 0 < \alpha |
| sum from i equals zero to infinity of f of i | \sum_{i=0}^\infty (f(i)) |


## The grammar

TalkTex uses its own TalkTex grammar. This grammar consists of *symbols* and
*rules* on how to combine symbols into *expressions*. A symbol is a translation
of a word, or a (usually small) string of words, to a chunk of latex source
code. An expression is a combination of symbols, concatinated in a particular
way, following the rules of the grammar. To clarify this, let us explain what a
*variable* is in the TalkTex grammar.

We denote a symbol as: "word(s)"|"latex". For example, we have the symbol
"left arrow"|"\leftarrow". This symbol expresses that TalkTex will translate the
words "left arrow" to the latex source code "\leftarrow". If either side of the
translation is a single word, we usually omit the quotes. For example,
limit|\lim is a symbol as well.

A **letter** is one of the following symbols: a|a, "capital a"|A, b|b,
"capital b"|B, ... z|z, "capital z"|Z, alpha|\alpha, beta|\beta, ...,
omega|\omega.

A **typesetting** is one of the following symbols: bold|\mathbb,
caligraphic|\mathcal, fraktur|\mathfrak.

An **accent** is one of the following symbols: tilde|\tilde, prime|\prime,
bar|\bar, hat|\hat.

A **variable** is an expression composed of letters, typesettings and accents
via the following rules:

- "letter[l]"|l
- "typesetting[t] variable[v]"|"t{v}"
- "variable[v] accent[a]"|"a{v}"

Hence if you speek the words "k hat", then TalkTex will see that "k" is a letter
and "hat" is an accent. According to the first rule above, every letter is in
particular also a variable. And according to the third rule above, the composed
string "k hat" is also a variable. The rule also explains how to translate such
a variable to Latex source code: [v] denotes that "v" is an alias for the latex
source code of the variable and [a] denotes that "a" is an alias for the latex
source code of the accent. After the pipe, it is specified that the latex
translation of the total new variable should be "a{v}". So in our example this
would be "\hat{k}".

The definitions above enable us to perform the following translations with
TalkTex:
| Speech | Latex |
| ----------- | ----------- |
| capital a | A |
| b tilde | \tilde{b} |
| caligraphic capital c | \mathcal{C} |
| bold capital q bar | \bar{\mathbb{Q}} |

### letter
- `a`|`a`, `b`|`b`, ..., `z`|`z`
- `capital a`|`A`, `capital b`|`B`, ..., `capital z`|`Z`
- `alpha`|`\alpha`, `beta`|`\beta`, ..., `omega`|`\omega`

### typesetting
- `bold`|`\mathbb`
- `caligraphic`|`\mathcal`
- `fraktur`|`\mathfrak`

### accent
- `tilde`|`\tilde`
- `prime`|`\prime`
- `bar`|`\bar`
- `hat`|`\hat`

### variable
- `letter[l]`|`l`
- `typesetting[t] variable[v]`|`t{v}`
- `variable[v] accent[a]`|`a{v}`

### uniop
- `square root`|`\sqrt`
- `sin`|`\sin`
- `cos`|`\cos`
- `tan`|`\tan`
- `exp`|`\exp`
- `log`|`\log`
- `negation`|`\neg`

### binop
- `plus`|`+`
- `minus`|`-`
- `times`|`\cdot`
- `divided by`|`/`
- `over`|`/`
- `divides`|`\mid`
- `equals`|`=`
- `not equal`|`\neq` (unrecognized words are ignored, hence this enables both
  "a does not equal b" and "a is not equal to b")
- `less than`|`<`
- `greater than`|`>`
- `less or equal`|`\leq`
- `greater or equal`|`\geq`
- `isomorphic`|`cong`
- `equivalent`|`\sim`
- `and`|`\wedge`
- `or`|`\vee`
- `implies`|`\implies`
- `union`|`\cup`
- `intersection`|`\cap`
- `set minus`|`\setminus`
- `subset`|`\subset`
- `in`|`\in`

### rangeop
- `sum`|`\sum`
- `product`|`\prod`
- `integral`|`\int`

### range
- `FROM expr[a] TO expr[b]`|`_{a}^{b}`

### openexpr
- `variable[v]`|`v`
- `uniop[o] expr[a]`|`o(a)`
- `expr[a] binop[o] expr[b]`|`a o b`
- `rangeop[o] range[r] expr[a]`|`{o}{r}(a)`

### end
- `END`|``
- `TIME_PAUSE`|`` (if you user stops talking for a moment)

### expr
- `openexpr[a] end[e]`|`a`
- `openexpr[a]`|`a`

\frac, ^,  
\forall, \exists, \neg, \Leftrightarrow
\emptyset,
\lim, 
\rightarrow, \leftarrow, \mapsto, \not{...}, \infty



A **translation** is a triple of: type, phrase, target
A **terminal** is the simplest kind of translation.
A **construct** is a combination of terminals, **kerwords** and other constructs.

Our **vocabulary** is the set of all phrases in terminals and keywords.

Parentheses: "open parenthesis" and "close parenthesis"

Ignore all non-recognized text. If these results in conflicts for specific cases, we include those cases explicitly in our vocabulary.

Synonyms: we keep a seperate database of sysonyms of phrases. These synonyms are also listed in the user spec next to the "canonical" phrase.

Timing: a pause in speech will be interpreted as the "END" keyword.

Precedence/priority of construct rules.