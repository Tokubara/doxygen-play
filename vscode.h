/**
 * \brief 设置rm以及reg(如果不为NULL的话)
 * \param rm rm表示r/m位翻译出的结果, 可能是地址/寄存器, 由modr/m位决定, 与opcode无关
 * \param reg 取决与opcode指示reg/op到底是reg还是op, 如果是op, 设为NULL, 否则会进行设置
 * \param load_rm_val load_val是对rm, reg的修饰
 * 
 * 设置可以从modr/m以及之后的byte能获取到的全部operand信息, 事实上这是剩下的译码的全部工作
 */
void read_ModR_M(vaddr_t *pc, Operand *rm, bool load_rm_val, Operand *reg, bool load_reg_val);