#!/bin/bash
# Verify the output of 'example' against the expected result:

if [ "$(./example)" = "$(cat ./testing/expected.txt)" ]; then
    echo "Test passed!"
else
    echo "*** Test failed!"
fi
