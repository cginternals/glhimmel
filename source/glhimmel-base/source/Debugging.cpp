#include <glhimmel-base/Debugging.h>
#include <globjects/Error.h>
#include <globjects/logging.h>

namespace glHimmel
{

	void logGLError(const std::string &at)
	{
#ifndef NDEBUG
		auto error = globjects::Error::get();

		if (error)
		{
			globjects::warning() << std::hex << int(error.code()) << ": "
				<< error.name() << " at " << at << std::endl;
		}
#endif
	}


} // namespace glHimmel