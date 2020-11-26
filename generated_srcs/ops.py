def type_to_dpcc_type(s):
    return type_to_dpcc_type.d[s]
type_to_dpcc_type.d = {
    "int": "TYPE_I32",
    "float": "TYPE_F32",
    "bool": "TYPE_BOOL",
    "int[]": "TYPE_I32_ARRAY",
    "float[]": "TYPE_F32_ARRAY",
}

class OpsBundle:
    def __init__(self, yytokentypes, types_conversion_table):
        self.yytokentypes = yytokentypes
        self.types_conversion_table = types_conversion_table


INTEGER_OPS = OpsBundle(
    [
        "TOK_MOD",
        "TOK_BNOT",
        "TOK_BAND",
        "TOK_BOR",
        "TOK_BXOR",
        "TOK_BLSHIFT",
        "TOK_BRSHIFT",
    ],
    [
        ["int", ["int", "int"]]
    ]
)


MATH_OPS = OpsBundle(
    [
        "TOK_ASSIGN",
        "TOK_ADD",
        "TOK_SUB",
        "TOK_MUL",
        "TOK_DIV",
        "TOK_POW",
        "TOK_INC",
        "TOK_DEC",
        "TOK_POS",
        "TOK_NEG",
    ],
    [
        ["int", ["int", "int"]],
        ["float", ["float", "int"]],
        ["float", ["int", "float"]],
        ["float", ["float", "float"]],

        ["int", ["int"]],
        ["float", ["float"]],
    ]
)


LOGICAL_COMPARISONS = OpsBundle(
    [

        "TOK_EQ",
        "TOK_NEQ",
        "TOK_GT",
        "TOK_GTEQ",
        "TOK_LTEQ",
    ],
    [
        ["bool", ["int", "int"]],
        ["bool", ["float", "int"]],
        ["bool", ["int", "float"]],
        ["bool", ["float", "float"]],

    ]
)


LOGICAL_OPERATORS = OpsBundle(
    [
        "TOK_LNOT",
        "TOK_LAND",
        "TOK_LOR",
    ],
    [
        ["bool", ["bool", "bool"]],
        ["bool", ["bool"]],
    ]
)


ARRAY_OPERATORS = OpsBundle (
    [
        "TOK_AR_SUBSCR",
    ],
    [
        ["int", ["int[]", "int"]],
        ["float", ["float[]", "int"]]
    ],
)


ALL_BUNDLES = [
    INTEGER_OPS,
    MATH_OPS,
    LOGICAL_COMPARISONS,
    LOGICAL_OPERATORS,
    ARRAY_OPERATORS
]


ALL_OPS = [
]

for bundle in ALL_BUNDLES:
    for yytokenstype in bundle.yytokentypes:
        ALL_OPS.append(yytokenstype)
