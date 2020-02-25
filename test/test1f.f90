program saydx_test1f
  use, intrinsic :: iso_fortran_env, only : stderr => error_unit
  use saydx

  call run_test()

contains

  subroutine run_test()
    type(eventprinter_t), target :: eventprinter
    class(eventhandler_t), pointer :: eventhandler
    type(msdparser_t) :: msdparser
    type(error_t) :: error

    call eventprinter_init(eventprinter)
    eventhandler => eventprinter
    call msdparser_init(msdparser, eventhandler)
    call msdparser%parse_file("test1.msd", error=error)
    if (error%is_associated()) then
      write(stderr, "(3A)") "Error during parsing file '", "test1.msd", "'"
      error stop 1
    end if

  end subroutine run_test

end program saydx_test1f
