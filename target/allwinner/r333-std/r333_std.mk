$(call inherit-product-if-exists, target/allwinner/r333-common/r333-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r333_std
PRODUCT_DEVICE := r333-std
PRODUCT_MODEL := Allwinner r333 std board
