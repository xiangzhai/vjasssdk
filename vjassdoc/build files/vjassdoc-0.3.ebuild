# Copyright 2009 Tamino Dauth
# Distributed under the terms of the GNU General Public License v2
# $Header: $

DESCRIPTION="Documentation tool for the scripting languages Jass and vJass."
SRC_URI="http://mesh.dl.sourceforge.net/project/vjasssdk/vjasssdk/${PN}/releases/${PN}-${PV}.7z"
HOMEPAGE="http://sourceforge.net/projects/vjasssdk/"
#KEYWORDS="" #Unknown
SLOT="0"
LICENSE="GPL-2"
#IUSE=""

RDEPEND="
	>=sys-devel/gettext-0.17
    >=dev-libs/boost-1.39.0
	>=sys-libs/glibc-2.9_p20081201-r2
	sqlite? (
		>=dev-db/sqlite-3.6.13
	)
	kde? (
        >=kde-base/kdelibs-4.3.1-r1
    )"

DEPEND="${RDEPEND}"
DOCS="AUTHORS COPYING ChangeLog NEWS README TODO doc/Datenbanken.txt"
#PDEPEND=""
#RESTRICT=""
#PROVIDE=""

src_configure()
{
	mycmakeargs="${mycmakeargs}
        $(cmake-utils_use_with sqlite kde)"

	cmake "${FILESDIR}/${P}" || die "cmake failed"
}

src_compile()
{
	make -C "${FILESDIR}/${P}"
}

src_install()
{
	make -C "${FILESDIR}/${P}" install
}


#pkg_setup  	 Use this function to perform any miscellaneous prerequisite tasks. This might include checking for an existing configuration file.
#pkg_nofetch 	Inform the user about required actions if for some reason (such as licensing issues) the sources may not be downloaded by Portage automatically. Use this in conjunction with RESTRICT="fetch". You only should display messages in this function, never call die.
#src_unpack 	Use this function to unpack your sources, apply patches, and run auxiliary programs such as the autotools. By default, this function unpacks the packages listed in A. The initial working directory is defined by WORKDIR.
#src_compile 	Use this function to configure and build the package. The initial working directory is S.
#src_install 	Use this function to install the package to an image in D. If your package uses automake, you can do this simply with emake DESTDIR="${D}" install. Make sure your package installs all its files using D as the root! The initial working directory is S.
#src_test 	Executed only when FEATURES="test" is set and RESTRICT="test" is unset, the default of this function executes an available testing function from any Makefiles in the ${S} directory, running either "make test" or "make check" depending on what is provided. It can be overriden to create a custom test setup.
#pkg_preinst 	The commands in this function are run just prior to merging a package image into the file system.
#pkg_postinst 	The commands in this function are run just following merging a package image into the file system.
#pkg_prerm 	The commands in this function are run just prior to unmerging a package image from the file system.
#pkg_postrm 	The commands in this function are run just following unmerging a package image from the file system.
#pkg_config 	You use this function to setup an initial configuration for the package after it's installed. All paths in this function should be prefixed with ROOT which points to user-specified install root which may not happen to be /. This function is only executed if and when the user runs: emerge --config =${PF}. 
