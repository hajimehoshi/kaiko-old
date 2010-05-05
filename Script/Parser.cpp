#include "Parser.hpp"

/*
 * CompilationUnit      := ( ClassDefinition )*
 * ClassDefinition      := ClassName { MethodDefinitions* }
 * MethodDefinition     := MethodName ( "(" Arguments ")" )? "{" Statement* "}"
 * Arguments            := Identifier ( "," Identifier )*
 *
 * Statement            := "if" IfStatementBody
 *                       | "for" ForStatementBody
 *                       | Expression ( "\n" | ";" )
 * IfStatementBody      := Expression "{" Statements "}" ( "else" "{" Statements "}" )?
 * ForStatementBody     := "{" Statements "}"
 *                       | Expression "{" Statements "}"
 *                       | Expression ";" Expression ";" Expression "{" Statements "}"
 *
 * Expression           := 
 * MethodCalling        := Primitive ()
 * PrimitiveExpression  := ConditionExpression
 * ConditionExpression  := OrExpression ( "?" OrExpression ":" OrExpression )?
 * OrExpression         := XorExpression ( "||" XorExpression )*
 * XorExpression        := AndExpression ( "^" AndExpression )*
 * AndExpression        := EqualsExpression ( "&&" EqualsExpression )*
 * EqualsExpression     := ComparisonExpression ( "==" ComparisonExpression | "!=" ComparisonExpression )*
 * ComparisonExpression := ShiftExpression ( "<" ShiftExpression | "<=" ShiftExpression | ">" ShiftExpression | ">=" ShiftExpression )*
 * ShiftExpression      := AdditionalExpression ( "<<" AdditionalExpression | ">>" AdditionalExpression | ">>>" AdditionalExpression | ">=" AdditionalExpression )*
 * AdditionalExpression := MultipleExpression ( "+" MultipleExpression | "-" MultipleExpression )*
 * MultipleExpression   := UnaryExpression ( "*" UnaryExpression | "/" UnaryExpression | "%" UnaryExpression )*
 * UnaryExpression      := Primitive | "!" UnaryExpression | "+" UnaryExpression | "-" UnaryExpression
 *
 * MethodInvocation     := 
 *
 * Primitive
 */

namespace Kaiko {
namespace Script {

Parser::Parser(Lexer& lexer)
  : lexer(lexer)
{
}

}
}
