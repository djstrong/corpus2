#!/bin/bash

find -type f -name "*.i" | xargs sed -i 's/^namespace Corpus2/\n%feature("autodoc", "1");\nnamespace Corpus2/'
