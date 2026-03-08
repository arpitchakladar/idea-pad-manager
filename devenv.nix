{ pkgs, ... }:

{
	env.hardeningDisable = [ "fortify" ];

	languages.cplusplus = {
		enable = true;
		lsp = {
			enable = true;
			package = pkgs.ccls;
		};
	};

	packages = with pkgs; [
		cmake
		clang-tools
		ninja
	];
}
