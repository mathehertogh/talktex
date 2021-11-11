# User guide to TalkTex

This document explains what a user of TalkTex should say to obtain certain
Latex output.


## The TalkTex grammar

TalkTex uses its own **TalkTex grammar**. This grammar defines how **phrases**,
which are strings of words spoken by the user, are translated to Latex source
code. In order to do so, TalkTex maintains its own **vocabulary**: the set of
minimal phrases which build up our grammar. The TalkTex vocabulary consists of
two kinds of phrases: **terminals** and **keywords**.

Terminals are (minimal) phrases which have a direct translation to Latex source
code, which we call the **target**. An example is "union", whose target is
"\cup". A minimal phrase may consist out of multiple words. For example we also
have the terminal "less than" whose target is "<". A key property of terminals
is that their translations do not depend on the context of the spoken words.

Keywords are (minimal) phrases which do not have a direct translation to Latex
source code, but which do contribute to the meaning of a spoken sentence. An
example is the keyword "end". This keyword is used to indicate where the scope
of a certain expression ends. Consider the phrase "sine of x plus one", which is
translated to "\sin(x+1)". If we would want to obtain "\sin(x)+1" as output, we
could say "sine of x end plus one". Here the keyword "end" indicates that the
scope of the expression of which we take the sine ends. Also note that the
keyword "end" itself does not have a direct translation into Latex source code.

Our grammar allows a user to build up more complicated sentences using terminals
and keywords. Rules for how to build up more complicated (parts of) sentences
are defined by **constructs**. A construct consists of five pieces of
information, called its **type**, **phrase**, **target**, **priority** and
**synonyms**. The type declares what kind of construct this is. The phrase
indicates the words spoken by the user and the target gives the corresponding
Latex source code translation. The priority indicates in which order this
construct should be applied with respect to other available constructs: higher
priority constructs will always be applied first. The synonyms indicate
alternative phrase that the user could say to also obtain the same Latex source
code. We clarify the above by means of examples below.


# Example: terminal constructs

We start off with the simplest kind of example, namely we will define two
**terminal constructs**. These are constructs defining terminals. Construct
definitions are denoted as follows.

## BINOP
| phrase        | target    | priority  | synonyms                   |
|---------------|-----------|-----------|----------------------------|
| divided by    | /         | 10        |                            |
| greater equal | \geq      | 11        | bigger equal, larger equal |
| equal         | =         | 10        | equals                     |

The definition above defines three constructs: the "divided by"-construct, the
"greater equal"-construct and the "equal"-construct. Their types are all
"BINOP", indicating that these are binary oparators. The first construct has
phrase "divided by" and target "/". This means that when a user says "divided
by", TalkTex will translate this to the Latex source code "/". This definition
hence also defines the phrase "divided by" to be a terminal. The "greater
equal"-construct has the higher priority 11, while the other two have priority
10. This ensures that we do not interpret the phrase "equal" as an equal sign if
the word "greater" directly precedes it. The "greater equal"-construct has two
synonyms: "bigger equal" and "larger equal". This defines these two phrases to
be terminals as well and both are translated to the Latex source code "/" as
well.


# Example: building constructs on top of constructs

Next we will look at general constructs. Next to terminals and keywords,
constructs may also use phrases of other constructs in their own phrases. As our
example we will consider VARIABLE-constructs (of type "VARIABLE"). For this we
first need a few new terminal constructs. We simplified the definition of LETTER
a little bit for this exposition. See the [reference](grammer.md) for the full
definition.

### LETTER
| phrase    | target | priority | synonyms |
|-----------|--------|----------|----------|
| a         | a      | 10       |          |
| capital a | A      | 10       |          |
| alpha     | \alpha | 10       |          |

### TYPESETTING
| phrase      | target   | priority | synonyms |
|-------------|----------|----------|----------|
| bold        | \mathbb  |10        |          |
| caligraphic | \mathcal |10        |          |

### ACCENT
| phrase | target | priority | synonyms |
|--------|--------|----------|----------|
| tilde  | \tilde | 10       |          |
| hat    | \hat   | 10       |          |
| bar    | \bar   | 10       |          |

Above we have defined LETTER, TYPESETTING and ACCENT constructs. There actually
exist more letters, accents and typesettings than the ones listed above. But we
keep it small here. Now we can define variables in TalkTex, i.e. constructs of
type VARIABLE:

### VARIABLE
| phrase                     | target | priority | synonyms |
|----------------------------|--------|----------|----------|
| LETTER[L]                  | L      | 9        |          |
| TYPESETTING[T] VARIABLE[V] | T{ V } | 8        |          |
| VARIABLE[V] ACCENT[A]      | A{ V } | 7        |          |

The phrases above contain words in all-caps. These all caps words denote a
construct of a specific type. For example the first construct defined above
(with priority 6), states that its phrase can be the phrase of any LETTER
construct. The notation [L] after LETTER creates an alias "L" for the target of
the corresponding LETTER construct. This alias is used in the target
specification. In this case it states that the target of this construct is the
same as the target of the LETTER construct that was spoken.
The phrase of the second construct consists of two parts: first a typesetting
and then a variable. Aliases "T" and "A" are declared for the targets of the
typesetting and the variable respectively. The target is defined in terms of
these aliases. Note that we use raw latex source code mixed with these aliases.
The constructs above provide us with the following translations.

| spoken phrase         | Latex source code translation |
|-----------------------|-------------------------------|
| alpha                 | \alpha                        |
| caligraphic capital a | \mathcal{ A }                 |
| alpha tilde           | \tilde{ \alpha }              |
| bold capital a hat    | \hat{ \mathbb{ A } }          |

Note that our priority definitions are relavant. If the second (i.e. priority 5)
construct would have had priority 3 instead, then the last translation would
have been "\mathbb{ \hat{ A } }".


# Example: constructs using keywords

As mentioned already, constructs may also be built using keywords. Let us give
an example of how keywords and constructs can be used together to build complex
constructs.

### OPENFUNC
| phrase                                           | target     | priority   | synonyms |
|--------------------------------------------------|------------|------------|----------|
| function VARIABLE[F] from SYMBOL[A] to SYMBOL[B] | F: A \to B | 5          |          |

Above, the words in small letters ("function", "from" and "to") are keywords.
These words should be pronounced literally by the user. In contrast, the
capitalized words (e.g. "VARIABLE") indicate other constructs.
Note also how we define three different aliases above ("F", "A" and "B"), all of
whose parent constructs are of type VARIABLE. This allows us to distinguish them
appropriately in the target definition. The definition above allows for example
the following translation.

| spoken phrase                                     | Latex source code translation   |
|---------------------------------------------------|---------------------------------|
| function f tilde from capital a to bold capital r | \tilde{ f }: A \to \mathbb{ R } |


# Pauses in speech

TalkTex recognizes pauses in your speech in such a way that it removes the
burden of saying "end" at the correct moments from the user. If a user takes
a small pause between saying consecutive words in a sentence, then TalkTex will
interpret that pause as if the user had said "end". This facilitates the
following behaviour. Below "..." denotes a pause in the users speech.

| spoken phrase         | Latex source code translation |
|-----------------------|-------------------------------|
| sin of a plus b       | \sin( a + b )                 |
| sin of a ... plus b   | \sin( a ) + b                 |


# Ignoring non-vocab words

TalkTex will only apply construct rules if the spoken words of the user contain
a substring of words that exactly match the phrase of the construct. For
example, if the user says "function f from a to b" then this will match the
phrase of the FUNCTION construct, while "function f g h from a to b" will not.

To allow the user to speak a bit more freely, we loosed the situation above a
bit. All words which are not in the TalkTex vocabulary (i.e. which are not
keywords or terminals) are ignored by TalkTex. This means that you can say
these so-called "non-vocab" words in between your phrases without affecting the
TalkTex translation. For example, the words "which" and "goes" are non-vocab
words and therefore the phrase "function f which goes from a to b" *will* be
recognized as a FUNCTION construct. As the words "which" and "goes" are ignored
by TalkTex, the phrase will be pre-parsed by TalkTex into the phrase "function f
from a to b", which does match the FUNCTION construct phrase exactly.

This allows for a broad range of "natural speech" to be blended into the phrases
you talk into TalkTex, such as the following.

| spoken phrase                                           | Latex source code translation |
|---------------------------------------------------------|-------------------------------|
| the sine of a ... is always greater or equal than alpha | \sin( a ) \geq \alpha         |

Again, "..." denotes a pause in speech.


# The complete TalkTex grammar

See the [complete TalkTex grammar document](grammar.md) for information on all
the vocabulary and grammar rules of TalkTex. These enable the user to *talk Tex*
in many ways. Let us list some examples below.

| spoken phrase                                                         | Latex source code translation   |
|-----------------------------------------------------------------------|---------------------------------|
| for all delta bigger than one we have that zero is smaller than alpha | \forall \alpha > 1 : 0 < \alpha |
| sum from i equals zero to infinity of f of i                          | \sum_{i=0}^\infty (f(i))        |