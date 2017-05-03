/** Copyright Sébastien Aupetit <sebtic@projectsforge.org> */

package org.projectsforge.teaching.codeanalyzerforc;

import java.io.FileInputStream;
import java.io.IOException;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import org.projectsforge.teaching.codeanalyzerforc.CollectVariablesListener.Pair;

public class Main {

  public static void main(String[] args) throws IOException {

    ANTLRInputStream input = new ANTLRInputStream(new FileInputStream(
        "/home/seb/articles/Enseignements/DI-0809/DI3-C-ProjetC-NextGen/analyzer/src/main/resources/MyString.c"));
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

    System.out.println();
    System.out.println();

    CollectFunctionListener collectFunctions = new CollectFunctionListener(parser);
    walker.walk(collectFunctions, tree);

  }

}
