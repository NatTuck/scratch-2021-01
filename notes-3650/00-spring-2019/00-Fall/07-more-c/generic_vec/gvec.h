
#define CONCAT(a, b) a ## _ ## b
#define SPEC1(type, name) CONCAT(type, name)
#define SPEC(name) SPEC1(TYPE, name)

typedef struct SPEC(vec) {
    int size;
    TYPE* data;
} SPEC(vec);

static
SPEC(vec)
SPEC(vec_alloc)(int nn)
{
    SPEC(vec) vv;
    vv.size = nn;
    vv.data = malloc(nn * sizeof(TYPE));
    return vv;
}

static
void
SPEC(vec_free)(SPEC(vec) pp)
{
    free(pp.data);
    pp.data = 0;
}

