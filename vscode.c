typedef struct {
  DHelper decode; ///< decode函数指针
  EHelper execute; ///< execute函数指针
  int width; ///< 宽度
} OpcodeEntry; ///< 操作数类型

/**
 * \brief 设置rm以及reg(如果不为NULL的话)
 * \param rm rm表示r/m位翻译出的结果, 可能是地址/寄存器, 由modr/m位决定, 与opcode无关
 * \param reg 取决与opcode指示reg/op到底是reg还是op, 如果是op, 设为NULL, 否则会进行设置
 * \param load_rm_val load_val是对rm, reg的修饰
 * 
 * 设置可以从modr/m以及之后的byte能获取到的全部operand信息, 事实上这是剩下的译码的全部工作
 */
void read_ModR_M(vaddr_t *pc, Operand *rm, bool load_rm_val, Operand *reg, bool load_reg_val) {
  ModR_M m; // <0> m用于保存读取到的modr/m byte
  m.val = instr_fetch(pc, 1);
  decinfo.isa.ext_opcode = m.opcode; // <1>
  if (reg != NULL) { // <2> 与前面opcode相关, 如果这个指令的opcode指示, reg/op位表示寄存器操作数, 不为NULL, 否则为NULL, 应解释为op位
    reg->type = OP_TYPE_REG;
    reg->reg = m.reg;
    if (load_reg_val) {
      rtl_lr(&reg->val, reg->reg, reg->width);
    }

#ifdef DEBUG
    snprintf(reg->str, OP_STR_SIZE, "%%%s", reg_name(reg->reg, reg->width));
#endif
  }

  if (m.mod == 3) { // 目的是寄存器
    rm->type = OP_TYPE_REG;
    rm->reg = m.R_M; // 寄存器号
    if (load_rm_val) {
      rtl_lr(&rm->val, m.R_M, rm->width);
    }

#ifdef DEBUG
    sprintf(rm->str, "%%%s", reg_name(m.R_M, rm->width));
#endif
  }
  else {
    load_addr(pc, &m, rm); // <3> 处理mod!=3的情况, 即ModR/M是地址, 会读取SIB和立即数字段(如果有的话), 存入rm->addr中
    if (load_rm_val) {
      rtl_lm(&rm->val, &rm->addr, rm->width);
    }
  }
}