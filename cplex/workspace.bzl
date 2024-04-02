"""Loads the nlopt library"""

# Sanitize a dependency so that it works correctly from code that includes
# Apollo as a submodule.
def clean_dep(dep):
    return str(Label(dep))

def repo():
    # nlopt
    native.new_local_repository(
        name = "cplex",
        build_file = clean_dep("//cplex:cplex.BUILD"),
        path = "/apollo/cplex/opl",
    )
