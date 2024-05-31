## Getting started
1. Install the nix package manager using  
```
curl --proto '=https' --tlsv1.2 -sSf -L https://install.determinate.systems/nix | sh -s -- install
```

2. Make sure the camera is plugged in

3. Run the command
```
nix run "github:seanybaggins/tiscamera-test#pkgs.tiscamera-test"
```
Note: No cloning of the repo is necsarry.

If you want to run the repo with your custom changes then run
```
nix run ".#pkgs.tiscamera-test"
```
