#include <cc/builder/expr.h>

CExpr cexpr_empty(void)
{
    return (CExpr){
        .type = CEXPR_EMPTY,
    };
}

CExpr cexpr_constant(CVal val)
{
    return (CExpr){
        .type = CEXPR_CONSTANT,
        .constant_ = val,
    };
}

CExpr cexpr_identifier(Str ident, Alloc *alloc)
{
    return (CExpr){
        .type = CEXPR_IDENTIFIER,
        .identifier_ = str_dup(ident, alloc)};
}

CExpr cexpr_call(Alloc *alloc, CExpr name)
{
    CExpr call;
    call.type = CEXPR_CALL;
    call.call_.expr = alloc_move(alloc, name);

    vec_init(&call.call_.args, alloc);

    return call;
}

void cexpr_add_arg(CExpr *self, CExpr arg)
{
    if (self->type != CEXPR_CALL)
    {
        panic$("cexpr add arg should only be called with a call expression");
    }

    vec_push(&self->call_.args, arg);
}

CExpr cexpr_infix(CExpr left, COp type, CExpr right, Alloc *alloc)
{
    return (CExpr){
        .type = CEXPR_INFIX,
        .infix_ = {
            .op = type,
            .lhs = alloc_move(alloc, left),
            .rhs = alloc_move(alloc, right),
        },
    };
}

CExpr cexpr_prefix(COp type, CExpr left, Alloc *alloc)
{
    return (CExpr){
        .type = CEXPR_PREFIX,
        .prefix_ = {
            .op = type,
            .expr = alloc_move(alloc, left),
        },
    };
}

CExpr cexpr_postfix(CExpr left, COp type, Alloc *alloc)
{
    return (CExpr){
        .type = CEXPR_POSTFIX,
        .postfix_ = {
            .op = type,
            .expr = alloc_move(alloc, left),
        },
    };
}

CExpr cexpr_cast(CExpr expr, CType type, Alloc *alloc)
{
    return (CExpr){
        .type = CEXPR_CAST,
        .cast_ =
            {
                .type = type,
                .expr = alloc_move(alloc, expr),
            },
    };
}

CExpr cexpr_ternary(CExpr cond, CExpr etrue, CExpr efalse, Alloc *alloc)
{
    return (CExpr){
        .type = CEXPR_TERNARY,
        .ternary_ =
            {
                .expr_cond = alloc_move(alloc, cond),
                .expr_true = alloc_move(alloc, etrue),
                .expr_false = alloc_move(alloc, efalse),
            },
    };
}

CExpr cexpr_array_initializer(Alloc *alloc)
{
    CExpr result = {
        .type = CEXPR_ARRAY_INITIALIZER,
    };

    vec_init(&result.array_initializer_.initializer, alloc);

    return result;
}

CExpr cexpr_struct_initializer(Alloc *alloc)
{
    CExpr result = {
        .type = CEXPR_STRUCT_INITIALIZER,
    };

    vec_init(&result.struct_initializer_.initializer, alloc);

    return result;
}

void cexpr_initializer_push_element(CExpr *target, CExpr designator, CExpr expr, Alloc *alloc)
{
    if (target->type == CEXPR_STRUCT_INITIALIZER || target->type == CEXPR_ARRAY_INITIALIZER)
    {
        CInitializer initializer = {.designator = alloc_move(alloc, designator), .initializer = alloc_move(alloc, expr)};
        vec_push(&target->struct_initializer_.initializer, initializer);
    }
    else
    {
        panic$("cepxr_initializer_push_element must be used with array initializer or struct initializer");
    }
}

// ++a
CExpr cexpr_preinc(CExpr a, Alloc *alloc)
{
    return cexpr_prefix(COP_INC, a, alloc);
}

// --a
CExpr cexpr_predec(CExpr a, Alloc *alloc)
{
    return cexpr_prefix(COP_DEC, a, alloc);
}

// a++
CExpr cexpr_postinc(CExpr a, Alloc *alloc)
{
    return cexpr_postfix(a, COP_INC, alloc);
}

// a--
CExpr cexpr_postdec(CExpr a, Alloc *alloc)
{
    return cexpr_postfix(a, COP_DEC, alloc);
}

// a = b
CExpr cexpr_assign(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_ASSIGN, b, alloc);
}

// a += b
CExpr cexpr_assign_add(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_ASSIGN_ADD, b, alloc);
}

// a -= b
CExpr cexpr_assign_sub(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_ASSIGN_SUB, b, alloc);
}

// a *= b
CExpr cexpr_assign_mul(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_ASSIGN_MULT, b, alloc);
}

// a /= b
CExpr cexpr_assign_div(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_ASSIGN_DIV, b, alloc);
}

// a %= b
CExpr cexpr_assign_mod(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_ASSIGN_MOD, b, alloc);
}

// a &= b
CExpr cexpr_assign_and(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_ASSIGN_BIT_AND, b, alloc);
}

// a |= b
CExpr cexpr_assign_or(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_ASSIGN_BIT_OR, b, alloc);
}

// a ^= b
CExpr cexpr_assign_xor(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_ASSIGN_BIT_XOR, b, alloc);
}

// a <<= b
CExpr cexpr_assign_lshift(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_ASSIGN_LSHIFT, b, alloc);
}

// a >>= b
CExpr cexpr_assign_rshift(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_ASSIGN_RSHIFT, b, alloc);
}

// a + b
CExpr cexpr_add(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_ADD, b, alloc);
}

// a - b
CExpr cexpr_sub(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_SUB, b, alloc);
}

// a * b
CExpr cexpr_mul(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_MULT, b, alloc);
}

// a / b
CExpr cexpr_div(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_DIV, b, alloc);
}

// a % b
CExpr cexpr_mod(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_MOD, b, alloc);
}

// ~(a)
CExpr cexpr_bit_not(CExpr a, Alloc *alloc)
{
    return cexpr_prefix(COP_BIT_NOT, a, alloc);
}

// a & b
CExpr cexpr_bit_and(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_BIT_AND, b, alloc);
}

// a | b
CExpr cexpr_bit_or(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_BIT_OR, b, alloc);
}

// a ^ b
CExpr cexpr_bit_xor(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_BIT_XOR, b, alloc);
}

// a << b
CExpr cexpr_lshift(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_LSHIFT, b, alloc);
}

// a >> b
CExpr cexpr_rshift(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_RSHIFT, b, alloc);
}

// !(a)
CExpr cexpr_not(CExpr a, Alloc *alloc)
{
    return cexpr_prefix(COP_NOT, a, alloc);
}

// a && b
CExpr cexpr_and(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_AND, b, alloc);
}

// a || b
CExpr cexpr_or(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_OR, b, alloc);
}

// a == b
CExpr cexpr_eq(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_EQ, b, alloc);
}

// a != b
CExpr cexpr_not_eq(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_NOT_EQ, b, alloc);
}

// a < b
CExpr cexpr_lower(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_LT, b, alloc);
}

// a > b
CExpr cexpr_greater(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_GT, b, alloc);
}

// a >= b
CExpr cexpr_greater_eq(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_GT_EQ, b, alloc);
}

// a <= b
CExpr cexpr_lower_eq(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_LT_EQ, b, alloc);
}

// a[b]
CExpr cexpr_index(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_INDEX, b, alloc);
}

// *a
CExpr cexpr_deref(CExpr a, Alloc *alloc)
{
    return cexpr_prefix(COP_DEREF, a, alloc);
}

// &a
CExpr cexpr_ref(CExpr a, Alloc *alloc)
{
    return cexpr_prefix(COP_REF, a, alloc);
}

// a.b
CExpr cexpr_access(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_ACCESS, b, alloc);
}

// a->b
CExpr cexpr_ptr_access(CExpr a, CExpr b, Alloc *alloc)
{
    return cexpr_infix(a, COP_PTR_ACCESS, b, alloc);
}