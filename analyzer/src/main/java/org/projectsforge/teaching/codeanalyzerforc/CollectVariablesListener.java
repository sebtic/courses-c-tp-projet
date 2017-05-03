/** Copyright Sébastien Aupetit <sebtic@projectsforge.org> */
package org.projectsforge.teaching.codeanalyzerforc;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import org.antlr.v4.runtime.ParserRuleContext;
import org.projectsforge.teaching.codeanalyzerforc.CParser.DeclarationContext;
import org.projectsforge.teaching.codeanalyzerforc.CParser.DeclarationSpecifiersContext;
import org.projectsforge.teaching.codeanalyzerforc.CParser.DeclaratorContext;
import org.projectsforge.teaching.codeanalyzerforc.CParser.InitDeclaratorContext;

class CollectVariablesListener extends CBaseListener {

  public static class Pair {
    String declaration;
    int occurence;

    Pair(String declaration) {
      this.declaration = declaration;
      this.occurence = 0;
    }

    @Override
    public boolean equals(Object obj) {
      return declaration.equals(((Pair) obj).declaration);
    }

    @Override
    public int hashCode() {
      return ((declaration == null) ? 0 : declaration.hashCode());
    }

    @Override
    public String toString() {
      return "" + occurence + "\t" + declaration;
    }
  }

  private CParser parser;
  private boolean inDeclaration = false;
  private Map<Pair, Pair> pairs = new HashMap<>();

  private String type;

  public CollectVariablesListener(CParser parser) {
    this.parser = parser;
  }

  @Override
  public void enterDeclaration(DeclarationContext ctx) {
    inDeclaration = true;
  }

  @Override
  public void enterEveryRule(ParserRuleContext ctx) {
    if (ctx instanceof DeclarationSpecifiersContext) {
      entryDeclarationSpecifiers((DeclarationSpecifiersContext) ctx);
    }
  }

  public void entryDeclarationSpecifiers(DeclarationSpecifiersContext ctx) {
    type = Util.asString(ctx, false);
  }

  @Override
  public void exitDeclaration(DeclarationContext ctx) {
    inDeclaration = false;
  }

  @Override
  public void exitDeclarator(DeclaratorContext ctx) {
    String asString = Util.asString(ctx, false);
    if (inDeclaration && ctx.getParent() instanceof InitDeclaratorContext) {

      if (ctx.getParent().getParent().getText().startsWith("(*")
          || !ctx.getParent().getParent().getText().startsWith("(")) {

        /*
         * for (int i = 0; i <
         * ctx.getParent().getParent().getParent().getChildCount(); ++i)
         * System.err.println("\"" +ctx.getParent().getParent().getParent
         * ().getChild(i).getText() + "\" => " +ctx.getParent().getParent
         * ().getParent().getChild(i).getClass().getSimpleName());
         */
        /*
         * System.err.println(ctx.getParent().getClass().getSimpleName() + "->"
         * + ctx.getParent().getParent().getClass().getSimpleName() + "->" +
         * ctx.getParent().getParent().getParent().getClass().getSimpleName ());
         * 
         * 
         * System.err.println("## " + ctx.getParent().getParent().getClass()
         * .getSimpleName() + " => " + ctx.getParent().getParent().getText() +
         * " => " + asString);
         */

        Pair pair = pairs.get(new Pair(asString));
        if (pair == null) {
          pair = new Pair(type + asString);
          pairs.put(pair, pair);
        }
        pair.occurence++;
      }
    }
  }

  public Set<Pair> getPairs() {
    return pairs.keySet();
  }
}
