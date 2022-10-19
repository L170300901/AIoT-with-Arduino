import React, {useState, useEffect, useContext} from 'react';
import {
  Box, Slider, Stack, Typography,
} from '@mui/material';
import axios from 'axios';
import GlobalContext from 'context/global';

import { FetchComponentBaseProps, FetchComponentWrapper } from './internal/FetchComponentWrapper';

import $ from './SliderControl.module.scss';

interface Props extends FetchComponentBaseProps {
  min: number
  max: number
  step?: number
  unit?: string
  description?: string
};

const SliderControl: React.FC<Props> = function (props: Props) {
  const {
    label, dataID, min, max, description, unit, step, action,
  } = props;
  const { items } = useContext(GlobalContext);
  const [disabled, setDisabled] = useState(true);
  const [value, setValue] = useState(0);

  useEffect(() => {
    if (dataID in items) {
      setValue(items[dataID].value);
    }
    setDisabled(false);
  }, [items, dataID]);

  const onChange = (_: any, v: any, __:any) => {
    if (typeof v === 'number') {
      setValue(v);
    }
  };

  const onChangeCommitted = (_: any, v: any) => {
    if (typeof v === 'number') {
      setDisabled(true);
      axios({
        method: 'post',
        url: '/api/save',
        headers: { 'Content-Type': 'text/plain' },
        data: `${dataID}=${v}`,
      }).then(() => {
        setDisabled(false);
      }).catch((e) => {
        setDisabled(false);
        console.error(e);
      });
    }
  };
  return (
    <FetchComponentWrapper
      className={$.container}
      label={label}
      action={action}
      dataID={dataID}
      dataFetchCount={1}
    >
      <Stack direction="row" spacing={3} alignItems="center" className={$.box}>
        <Typography variant="h6">{`${0}${unit}`}</Typography>
        <Box width={300}>
          <Slider
            step={step}
            onChange={onChange}
            onChangeCommitted={onChangeCommitted}
            disabled={disabled}
            valueLabelDisplay="on"
            valueLabelFormat={(value-min)/10}
            min={min}
            max={max}
            value={value}
            marks

          />
        </Box>
        <Typography variant="h6">{`${5}${unit}`}</Typography>
      </Stack>
    </FetchComponentWrapper>
  );
};

SliderControl.defaultProps = {
  description: '',
  unit: '',
  step: 10,
};

export default SliderControl;
