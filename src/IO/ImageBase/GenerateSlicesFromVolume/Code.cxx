/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkImageSeriesWriter.h"

int main( int argc, char* argv[] )
{
  if( argc != 3 )
    {
    std::cerr << "Usage: "<< std::endl;
    std::cerr << argv[0];
    std::cerr << " <InputFileName> <OutputFileName>";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }

  const char * inputFileName = argv[1];
  const char * outputFileName = argv[2];

  std::string format =  std::string( outputFileName ) +
                        std::string( "-%d.png" );

  const unsigned int Dimension = 3;

  typedef unsigned char                      PixelType;
  typedef itk::Image< PixelType, Dimension > InputImageType;

  typedef itk::ImageFileReader< InputImageType >  ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFileName );

  typedef unsigned char OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension > RescaleImageType;

  typedef itk::RescaleIntensityImageFilter< InputImageType, RescaleImageType >
    RescaleFilterType;
  RescaleFilterType::Pointer rescale = RescaleFilterType::New();
  rescale->SetInput( reader->GetOutput() );
  rescale->SetOutputMinimum( 0 );
  rescale->SetOutputMaximum( 255 );
  rescale->UpdateLargestPossibleRegion();

  InputImageType::RegionType region =
    reader->GetOutput()->GetLargestPossibleRegion();
  InputImageType::SizeType size = region.GetSize();

  itk::NumericSeriesFileNames::Pointer fnames =
    itk::NumericSeriesFileNames::New();
  fnames->SetStartIndex( 0 );
  fnames->SetEndIndex( size[2] - 1 );
  fnames->SetIncrementIndex( 1 );
  fnames->SetSeriesFormat( format.c_str() );

  typedef itk::Image< OutputPixelType, 2 > OutputImageType;

  typedef itk::ImageSeriesWriter< RescaleImageType, OutputImageType >
    WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( rescale->GetOutput() );
  writer->SetFileNames( fnames->GetFileNames() );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
