/** Copyright Sébastien Aupetit <sebtic@projectsforge.org> */

package org.projectsforge.teaching.codeanalyzerforc;

import org.projectsforge.teaching.codeanalyzerforc.CParser.FunctionDefinitionContext;

class CollectFunctionListener extends CBaseListener {

  private CParser parser;

  public CollectFunctionListener(CParser parser) {
    this.parser = parser;
  }

  @Override
  public void enterFunctionDefinition(FunctionDefinitionContext ctx) {

    System.out.println("##" + ctx.declarator().directDeclarator().directDeclarator().getText() + "## "
        + Util.asString(ctx.compoundStatement(), false));

  }

}
