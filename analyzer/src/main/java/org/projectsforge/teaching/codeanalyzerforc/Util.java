/** Copyright Sébastien Aupetit <sebtic@projectsforge.org> */

package org.projectsforge.teaching.codeanalyzerforc;

import org.antlr.v4.runtime.RuleContext;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.TerminalNodeImpl;
import org.projectsforge.teaching.codeanalyzerforc.CParser.DirectDeclaratorContext;

public class Util {
  public static String asString(ParseTree tree, boolean substituteIdentifiers) {
    if (substituteIdentifiers && tree instanceof DirectDeclaratorContext) {
      return "IDENTIFIER";
    } else if (tree instanceof RuleContext) {
      if (tree.getChildCount() == 0) {
        return "";
      }

      String result = "";
      for (int i = 0; i < tree.getChildCount(); i++) {
        result = (result + asString(tree.getChild(i), substituteIdentifiers)).trim() + ' ';
      }
      return result;
    } else if (tree instanceof TerminalNodeImpl) {
      return ((TerminalNodeImpl) tree).getText();
    } else {
      System.err.println("Unknown type " + tree.getClass().getSimpleName());
      return "";
    }
  }
}
