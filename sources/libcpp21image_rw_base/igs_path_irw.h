#ifndef igs_path_irw_h
#define igs_path_irw_h

#include <string> /* std::string */

#ifndef IGS_PATH_IRW_EXPORT
# define IGS_PATH_IRW_EXPORT
#endif

namespace igs {
 namespace path {
  /* ファイルパスから拡張子を得る */
  IGS_PATH_IRW_EXPORT const std::string ext_from_path(
	const std::string& file_path
  );
 }
}

#endif /* !igs_path_irw_h */
