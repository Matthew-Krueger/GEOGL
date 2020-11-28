# Organize args
if [ "$1" != "JenkinsRun" ]; then
  exit
fi

echo "Branch name is ${BRANCH_NAME}"

DOXYGEN_EXPORT_PATH="/var/www/docs.matthewkrueger.com/html/ci-doxygen/GEOGL/${BRANCH_NAME}"
rm -rf "$DOXYGEN_EXPORT_PATH" || exit
mkdir -p "$DOXYGEN_EXPORT_PATH" || exit

echo "Copying Doxygen to web server"
cp -r Docs/html/* "$DOXYGEN_EXPORT_PATH"
echo "Doxygen Copied to web server"