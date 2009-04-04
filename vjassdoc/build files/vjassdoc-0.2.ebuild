# Copyright 1999-2005 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

DESCRIPTION="A documentation tool for the scripting languages Jass and vJass."
SRC_URI="http://cdauth.de/~tamino/${PN}/${PN}-${MY_PV}.tar.bz2"
HOMEPAGE="http://cdauth.de/~tamino/${PN}"
#KEYWORDS="" #Unknown
SLOT="0"
LICENSE="GPL-2"
#IUSE=""

DEPEND="dev-db/sqlite
	sys-devel/gettext" #Check the versions
RDEPEND="dev-db/sqlite"
#PDEPEND=""
#RESTRICT=""
#PROVIDE=""


pkg_setup  	 Use this function to perform any miscellaneous prerequisite tasks. This might include checking for an existing configuration file.
pkg_nofetch 	Inform the user about required actions if for some reason (such as licensing issues) the sources may not be downloaded by Portage automatically. Use this in conjunction with RESTRICT="fetch". You only should display messages in this function, never call die.
src_unpack 	Use this function to unpack your sources, apply patches, and run auxiliary programs such as the autotools. By default, this function unpacks the packages listed in A. The initial working directory is defined by WORKDIR.
src_compile 	Use this function to configure and build the package. The initial working directory is S.
src_install 	Use this function to install the package to an image in D. If your package uses automake, you can do this simply with emake DESTDIR="${D}" install. Make sure your package installs all its files using D as the root! The initial working directory is S.
src_test 	Executed only when FEATURES="test" is set and RESTRICT="test" is unset, the default of this function executes an available testing function from any Makefiles in the ${S} directory, running either "make test" or "make check" depending on what is provided. It can be overriden to create a custom test setup.
pkg_preinst 	The commands in this function are run just prior to merging a package image into the file system.
pkg_postinst 	The commands in this function are run just following merging a package image into the file system.
pkg_prerm 	The commands in this function are run just prior to unmerging a package image from the file system.
pkg_postrm 	The commands in this function are run just following unmerging a package image from the file system.
pkg_config 	You use this function to setup an initial configuration for the package after it's installed. All paths in this function should be prefixed with ROOT which points to user-specified install root which may not happen to be /. This function is only executed if and when the user runs: emerge --config =${PF}. 
