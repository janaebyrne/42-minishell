#!/bin/bash

# Trap SIGINT
trap 'echo "Caught SIGINT, cleaning up..."; exit 1' SIGINT

echo "Script running in noninteractive mode..."
sleep 10  # Simulate long-running task
echo "Finished successfully!"

