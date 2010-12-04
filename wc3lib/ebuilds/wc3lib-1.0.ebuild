# Copyright 1999-2005 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

DESCRIPTION="Various libraries for using Blizzard's Warcraft 3 The Frozen Throne formats."
HOMEPAGE="http://sourceforge.net/projects/vjasssdk/"
#SRC_URI="mirror://sourceforge/ctags/${P}.tar.gz"

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="~x86"
IUSE="-editor"
#-html -sqlite -jpeg -tga -png -blend -max
DEPEND="
	>=app-arch/bzip2-1.0.5-r1
	>=sys-libs/zlib-1.2.3-r1

	#sqlite? ( >=dev-db/sqlite-3.6.23 )
	#jpeg? ( >=media-libs/openjpeg-1.3-r2 )
	#png? ( >=media-libs/libpng-1.2.40 )
	doc? ( >=app-doc/doxygen-1.5.8-r1)

	editor? ( >=x11-libs/qt-gui-4.6.2 >=kde-base/kdelibs-4.4.1-r1 )
"
# editor should be a local useflag

src_compile()
{
	econf --with-posix-regex
	emake || die "emake failed"
}

src_configure()
{
	mycmakeargs=(
		$(cmake-utils_use_with html sqlite)
	)
}

src_install() {
    emake DESTDIR="${D}" install || die "install failed"

    dodoc ChangeLog TODO README COPYING || die
    #dohtml EXTENDING.html ctags.html
    # libraries
    doman doc/man/wc3lib.en_US.3
    # applications
    doman doc/man/mpq.en_US.1
    doman doc/man/converter.en_US.1
    # file formats
    doman doc/man/mdx.en_US.5
    doman doc/man/mdl.en_US.5
    doman doc/man/blp.en_US.5
    doman doc/man/mpq.en_US.5
    # doman foo.1
    # will go into /usr/share/man/man1/foo.1
    # doman foo.lang.1
    # will go into /usr/share/man/lang/man1/foo.1 with EAPI="2"
}
 
