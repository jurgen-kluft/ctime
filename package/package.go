package ctime

import (
	cbase "github.com/jurgen-kluft/cbase/package"
	denv "github.com/jurgen-kluft/ccode/denv"
	ccore "github.com/jurgen-kluft/ccore/package"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// GetPackage returns the package object of 'ctime'
func GetPackage() *denv.Package {
	// Dependencies
	cunittestpkg := cunittest.GetPackage()
	ccorepkg := ccore.GetPackage()
	cbasepkg := cbase.GetPackage()

	// The main (ctime) package
	mainpkg := denv.NewPackage("ctime")
	mainpkg.AddPackage(cunittestpkg)
	mainpkg.AddPackage(ccorepkg)

	// 'ctime' library
	mainlib := denv.SetupCppLibProject("ctime", "github.com\\jurgen-kluft\\ctime")
	mainlib.AddDependencies(ccorepkg.GetMainLib()...)

	// 'ctime' unittest project
	maintest := denv.SetupDefaultCppTestProject("ctime_test", "github.com\\jurgen-kluft\\ctime")
	maintest.AddDependencies(cunittestpkg.GetMainLib()...)
	maintest.AddDependencies(ccorepkg.GetMainLib()...)
	maintest.AddDependencies(cbasepkg.GetMainLib()...)
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
