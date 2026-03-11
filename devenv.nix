{ pkgs, ... }:

{
	env = {
		CC = "${pkgs.clang}/bin/clang";
		CXX = "${pkgs.clang}/bin/clang++";
		CMAKE_EXPORT_COMPILE_COMMANDS = "1";
		hardeningDisable = [ "fortify" ];
	};

	languages.cplusplus = {
		enable = true;
	};

	packages = with pkgs; [
		cmake
		clang-tools
		ninja
		clang
		python3
		include-what-you-use
		clang-analyzer
		llvmPackages.libcxxClang
	];

	enterShell = ''
		export CC=${pkgs.clang}/bin/clang
		export CXX=${pkgs.clang}/bin/clang++
	'';
}
