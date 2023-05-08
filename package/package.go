package ctime

import (
	ccore "github.com/jurgen-kluft/ccore/package"
	denv "github.com/jurgen-kluft/ccode/denv"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// GetPackage returns the package object of 'ctime'
func GetPackage() *denv.Package {
	// Dependencies
	cunittestpkg := cunittest.GetPackage()
	ccorepkg := ccore.GetPackage()

	// The main (ctime) package
	mainpkg := denv.NewPackage("ctime")
	mainpkg.AddPackage(cunittestpkg)
	mainpkg.AddPackage(ccorepkg)

	// 'ctime' library
	mainlib := denv.SetupDefaultCppLibProject("ctime", "github.com\\jurgen-kluft\\ctime")
	mainlib.Dependencies = append(mainlib.Dependencies, ccorepkg.GetMainLib())

	// 'ctime' unittest project
	maintest := denv.SetupDefaultCppTestProject("ctime_test", "github.com\\jurgen-kluft\\ctime")
	maintest.Dependencies = append(maintest.Dependencies, cunittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, ccorepkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
