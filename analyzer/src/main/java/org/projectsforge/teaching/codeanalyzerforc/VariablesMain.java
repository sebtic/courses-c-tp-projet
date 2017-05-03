/** Copyright Sébastien Aupetit <sebtic@projectsforge.org> */

package org.projectsforge.teaching.codeanalyzerforc;

import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import org.projectsforge.teaching.codeanalyzerforc.CollectVariablesListener.Pair;

/** Main entry point to print detected variables in source codes */
public class VariablesMain {

  public static void main(String[] args) throws IOException {
    InputStreamReader in = new InputStreamReader(System.in, Charset.forName("UTF-8"));
    ANTLRInputStream input = new ANTLRInputStream(in);
    CLexer lexer = new CLexer(input);
    CommonTokenStream tokens = new CommonTokenStream(lexer);
    CParser parser = new CParser(tokens);

    ParseTree tree = parser.compilationUnit();
    ParseTreeWalker walker = new ParseTreeWalker();

    CollectVariablesListener collectVariables = new CollectVariablesListener(parser);

    walker.walk(collectVariables, tree);

    for (Pair pair : collectVariables.getPairs()) {
      System.out.println(pair);
    }
  }

}
