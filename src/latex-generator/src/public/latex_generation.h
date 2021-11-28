#pragma once

#include <string>
#include "syntax_tree.h"

namespace generation {

/** Returns the LaTeX math code needed to render the expression tree that [t] points to */
std::string to_latex(Syntax_tree::const_traverser t);

/**
 * Converts LaTeX math code into the code needed to render it in display style.
 * The resulting string should be used in LaTeX text mode.
 */
std::string to_display_style(const std::string& latex_expression);

/** Returns the TalkTeX LaTeX header, up to and including \begin{document} */
std::string talktex_header();

/** Returns the TalkTeX LaTeX footer, starting at \end{document} */
std::string talktex_footer();

} // namespace generation
