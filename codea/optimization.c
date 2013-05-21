
#include <stdlib.h>
#include "optimization.h"

struct code *find_imm (struct code *input)
{
	struct code *c;

	if (input == NULL)
		return NULL;

	if (OP_LABEL (input) == TT_NUM)
		return input;

	c = find_imm (LC (input));
	if (c != NULL)
		return c;

	return find_imm (RC (input));
}

struct code *optimize_immediate (struct code *code, int l_imm, int r_imm)
{
	int imm;
	struct code *imm_code;

	if (l_imm == 0 && r_imm == 0)
		return code;

	// Optimize add, sub and mult
	if (OP_LABEL (code) != TT_ADD && OP_LABEL (code) != TT_MULT && OP_LABEL (code) != TT_SUB) 
		return code;

	// Only handle complex cases, easy ones (only imms is handled by iburg)
	if (l_imm != 0 && r_imm == 0) {
		imm = LC_VAL (code);
		imm_code = find_imm (RC (code));
	} else if (l_imm == 0 && r_imm != 0) {
		imm = RC_VAL (code);
		imm_code = find_imm (LC (code));
	} else {
		return code;
	}

	if (imm_code == NULL)
		return code;

	switch (OP_LABEL (code)) {
		case TT_ADD:
			VAL (imm_code) += imm;
			break;
		case TT_MULT:
			VAL (imm_code) *= imm;
			break;
		case TT_SUB:
			VAL (imm_code) += imm;
			break;
		default:
			return code;
	}

	if (l_imm != 0)
		return RC (code);
	else
		return LC (code);
}
