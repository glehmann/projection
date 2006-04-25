WRAP_CLASS("itk::MaximumProjectionImageFilter" POINTER_WITH_SUPERCLASS)
  # increment the output dimension by 1, if possible
  FOREACH(t ${WRAP_ITK_SCALAR})
    FOREACH(d ${WRAP_ITK_DIMS})
      INCREMENT(d1 ${d})
      FILTER_DIMS(d2 "${d1}")
      IF(d2)
        WRAP_TEMPLATE("${ITKM_I${t}${d}}${ITKM_I${t}${d2}}" "${ITKT_I${t}${d}},${ITKT_I${t}${d2}}")
      ENDIF(d2)
    ENDFOREACH(d)
  ENDFOREACH(t)
END_WRAP_CLASS()
