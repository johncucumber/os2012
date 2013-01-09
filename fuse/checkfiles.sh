  if ((diff $1 $2) &> /dev/null) && ((diff $2 $1) &> /dev/null); then
    echo "OK"
  else
    echo "FAIL"
  fi
