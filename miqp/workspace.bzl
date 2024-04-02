"""Loads the nlopt library"""

# Sanitize a dependency so that it works correctly from code that includes
# Apollo as a submodule.
def clean_dep(dep):
    return str(Label(dep))

def repo():
    # nlopt
    native.new_local_repository(
        name = "miqp_planner",
        build_file = clean_dep("//miqp:miqp.BUILD"),
        path = "/apollo/miqp",
    )
