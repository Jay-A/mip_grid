# conf.py
import os
import sys

# Add any paths that contain custom extensions or modules here
# (not needed for pure C++ API)
sys.path.insert(0, os.path.abspath('.'))

# -- Project information -----------------------------------------------------
project = 'Synthetic Grid'
author = 'Your Name or Organization'
release = '0.1'

# -- General configuration ---------------------------------------------------
extensions = [
    'breathe',                 # Bridge Doxygen XML b
]

templates_path = []            # optional
exclude_patterns = []          # optional

# -- Breathe configuration ---------------------------------------------------
breathe_projects = {
    "SyntheticGrid": "../xml"  # relative path from sphinx/conf.py to Doxygen XML
}
breathe_default_project = "SyntheticGrid"

# -- Options for HTML output -------------------------------------------------
html_theme = 'sphinx_rtd_theme'  # Clean, professional, widely used
html_title = "Synthetic Grid API"
html_theme_options = {
    "navigation_depth": 3,
    "collapse_navigation": False,
}
