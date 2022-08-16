package xtime

import (
	"github.com/jurgen-kluft/ccode/denv"
	"github.com/jurgen-kluft/xbase/package"
)

// GetPackage returns the package object of 'xtime'
func GetPackage() *denv.Package {
	// Dependencies
	xunittestpkg := xunittest.GetPackage()
	xbasepkg := xbase.GetPackage()

	// The main (xtime) package
	mainpkg := denv.NewPackage("xtime")
	mainpkg.AddPackage(xunittestpkg)
	mainpkg.AddPackage(xbasepkg)

	// 'xtime' library
	mainlib := denv.SetupDefaultCppLibProject("xtime", "github.com\\jurgen-kluft\\xtime")
	mainlib.Dependencies = append(mainlib.Dependencies, xbasepkg.GetMainLib())

	// 'xtime' unittest project
	maintest := denv.SetupDefaultCppTestProject("xtime_test", "github.com\\jurgen-kluft\\xtime")
	maintest.Dependencies = append(maintest.Dependencies, xunittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, xbasepkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
