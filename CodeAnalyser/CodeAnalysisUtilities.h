#pragma once
/////////////////////////////////////////////////////////////////////////////////
// CodeAnalysisUtilities.h - injection Codes Utilities                         //
// ver 1.0                                                                     //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                        //
/////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This .h file provide some functions which are providing injection codes.
 *
 * Required Files:
 * ---------------
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 8 Mar 2019
 * - first release
 *
*/

# include <string>

namespace CodeAnalysis {
	inline std::string functionTag(std::string s = "") { return "<span class=\"function\"" + s + ">"; };
	inline std::string classTag(std::string s = "") { return "<span class=\"class\"" + s + ">"; };
	inline std::string singleLineCommentTag(std::string s = "") { return "<span class=\"SingleLineComment\"" + s + ">"; };
	inline std::string multiLineCommentTag(std::string s = "") { return "<span class=\"MultiLineComment\"" + s + ">"; };
	inline std::string endTag() { return "</span>"; };
	inline std::string customTag(std::string className = "", std::string s = "") { return "<span class=\"" + className + "\"" + s + ">"; };
	inline std::string button(int id = -1, std::string s = "") { return "<button onClick=\"controlId(" + std::to_string(id) + ")\"" + s + ">+</button>"; };
}