/** Copyright Sébastien Aupetit <sebtic@projectsforge.org> */

package org.projectsforge.teaching.codeanalyzerforc;

import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

/** Main entry point to print detected functions in source codes */
public class FunctionsMain {

  public static void main(String[] args) throws IOException {
    InputStreamReader in = new InputStreamReader(System.in, Charset.forName("UTF-8"));
    ANTLRInputStream input = new ANTLRInputStream(in);
    CLexer lexer = new CLexer(input);
    CommonTokenStream tokens = new CommonTokenStream(lexer);
    CParser parser = new CParser(tokens);

    ParseTree tree = parser.compilationUnit();
    ParseTreeWalker walker = new ParseTreeWalker();

    CollectFunctionListener collectVariables = new CollectFunctionListener(parser);

    walker.walk(collectVariables, tree);
  }
}
