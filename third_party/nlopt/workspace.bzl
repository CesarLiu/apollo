"""Loads the nlopt library"""

# Sanitize a dependency so that it works correctly from code that includes
# Apollo as a submodule.
def clean_dep(dep):
    return str(Label(dep))

def repo():
    # nlopt
    native.new_local_repository(
        name = "nlopt",
        build_file = clean_dep("//third_party/nlopt:nlopt.BUILD"),
        path = "/usr/local",
    )
