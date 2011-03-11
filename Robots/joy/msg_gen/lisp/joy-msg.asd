
(cl:in-package :asdf)

(defsystem "joy-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Joy" :depends-on ("_package_Joy"))
    (:file "_package_Joy" :depends-on ("_package"))
  ))