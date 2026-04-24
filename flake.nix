{
  description = "development flake for idea-pad-manager";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    devenv = {
      url = "github:cachix/devenv";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  nixConfig = {
    extra-trusted-public-keys = "devenv.cachix.org-1:w1cLUi8dv3hnoSPGAuibQv+f9TZLr6cv/Hm9XgU50cw=";
    extra-substituters = "https://devenv.cachix.org";
  };

  outputs =
    {
      nixpkgs,
      devenv,
      ...
    }@inputs:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in
    {
      devShells.${system}.default = devenv.lib.mkShell {
        inherit inputs pkgs;
        modules = [
          (
            { pkgs, ... }:
            {
              git-hooks.hooks = {
                nixfmt.enable = true;
                format-script = {
                  enable = true;
                  name = "format project";
                  entry = "./scripts/format.sh";
                  language = "script";
                  pass_filenames = false;
                };
              };

              env = {
                CC = "${pkgs.clang}/bin/clang";
                CXX = "${pkgs.clang}/bin/clang++";
                CMAKE_EXPORT_COMPILE_COMMANDS = "1";
                hardeningDisable = [ "fortify" ];
              };

              languages.cplusplus.enable = true;

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
          )
        ];
      };
    };
}
