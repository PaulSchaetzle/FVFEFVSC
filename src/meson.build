fvfefvsc_sources = [
  'main.c',
  'fvfefvsc-application.c',
  'fvfefvsc-window.c',
]

fvfefvsc_deps = [
  dependency('gtk4'),
  dependency('libadwaita-1'),
]

fvfefvsc_sources += gnome.compile_resources('fvfefvsc-resources',
  'fvfefvsc.gresource.xml',
  c_name: 'fvfefvsc'
)

executable('fvfefvsc', fvfefvsc_sources,
  dependencies: fvfefvsc_deps,
       install: true,
)
