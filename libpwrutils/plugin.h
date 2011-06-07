#ifndef LIBPWRNLP_PLUGIN_H
#define LIBPWRNLP_PLUGIN_H

#include <boost/function.hpp>

namespace PwrNlp {
namespace Plugin {

/**
 * Convert a plugin name to a shared library name that is expected to
 * contain the plugin.
 */
std::string make_soname(const std::string& scope, const std::string& name);

/**
 * Load a plugin
 */
bool load(const std::string& scope, const std::string& name, bool quiet);

/**
 * Load a plugin, checking if a counter increases after the load,
 * and outputting a disgnostic message if it does not
 */
bool load_check(const std::string& scope, const std::string& name, bool quiet,
		boost::function<size_t(void)> counter, const std::string& what);

} /* end ns Plugin */
} /* end ns PwrNlp */

#endif // LIBPWRNLP_PLUGIN_H
